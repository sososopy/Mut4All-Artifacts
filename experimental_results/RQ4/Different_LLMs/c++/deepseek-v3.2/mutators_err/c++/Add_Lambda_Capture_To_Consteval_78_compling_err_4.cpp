//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Lambda_Capture_To_Consteval_78
 */ 
class MutatorFrontendAction_78 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(78)

private:
    class MutatorASTConsumer_78 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_78(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::VarDecl *> localVars;
    };
};

//source file
#include "../include/Mutator_78.h"

// ========================================================================================================
#define MUT78_OUTPUT 1

void MutatorFrontendAction_78::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Check if the call is to a consteval function
      const clang::FunctionDecl *FD = CE->getDirectCallee();
      if (!FD || !FD->isConsteval())
        return;
      
      // Find lambda argument
      clang::Expr *lambdaArg = nullptr;
      unsigned numArgs = CE->getNumArgs();
      for (unsigned i = 0; i < numArgs; ++i) {
        const clang::Expr *arg = CE->getArg(i);
        if (arg && arg->getStmtClass() == clang::Stmt::LambdaExprClass) {
          lambdaArg = const_cast<clang::Expr*>(arg);
          break;
        }
      }
      
      if (!lambdaArg)
        return;
      
      clang::LambdaExpr *lambda = clang::dyn_cast<clang::LambdaExpr>(lambdaArg);
      if (!lambda)
        return;
      
      // Check if lambda already has captures
      if (lambda->capture_size() > 0)
        return;
      
      // Get the function scope containing the call
      const clang::DeclContext *DC = Result.Context->getParents(*CE)[0].get<clang::DeclContext>();
      if (!DC)
        return;
      
      // Collect local variables in the scope
      localVars.clear();
      const clang::DeclContext *currentDC = DC;
      while (currentDC) {
        if (const clang::FunctionDecl *Func = clang::dyn_cast<clang::FunctionDecl>(currentDC)) {
          for (clang::Decl *D : Func->decls()) {
            if (clang::VarDecl *VD = clang::dyn_cast<clang::VarDecl>(D)) {
              if (VD->hasLocalStorage() && !VD->getType().isConstQualified()) {
                localVars.push_back(VD);
              }
            }
          }
        }
        currentDC = currentDC->getParent();
      }
      
      // Prepare capture variable
      std::string captureVar;
      bool needNewVar = false;
      
      if (!localVars.empty()) {
        // Choose a random local variable
        size_t idx = getrandom::getRandomIndex(localVars.size() - 1);
        captureVar = localVars[idx]->getNameAsString();
      } else {
        // No suitable local variable, introduce a new one
        needNewVar = true;
        captureVar = "mut78_x";
      }
      
      // Get lambda source range
      clang::SourceRange lambdaRange = lambda->getSourceRange();
      clang::SourceLocation lambdaStart = lambdaRange.getBegin();
      
      // Insert new variable if needed
      if (needNewVar) {
        std::string newVarDecl = "int " + captureVar + " = 0;\n";
        clang::SourceLocation insertLoc = lambdaStart;
        // Find the start of the statement containing the lambda
        clang::SourceManager &SM = Rewrite.getSourceMgr();
        clang::PresumedLoc PLoc = SM.getPresumedLoc(lambdaStart);
        clang::SourceLocation lineStart = SM.translateLineCol(SM.getMainFileID(), PLoc.getLine(), 1);
        Rewrite.InsertText(lineStart, newVarDecl);
      }
      
      // Modify lambda to add capture
      std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      size_t bracketPos = lambdaText.find('[');
      if (bracketPos != std::string::npos) {
        lambdaText.insert(bracketPos + 1, "&" + captureVar);
        lambdaText = "/*mut78*/" + lambdaText;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
      }
    }
}
  
void MutatorFrontendAction_78::MutatorASTConsumer_78::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(isConsteval())), hasAnyArgument(hasDescendant(lambdaExpr().bind("lambda")))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}