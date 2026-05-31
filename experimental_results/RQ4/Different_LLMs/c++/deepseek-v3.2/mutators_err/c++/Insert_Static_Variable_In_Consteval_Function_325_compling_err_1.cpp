//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Static_Variable_In_Consteval_Function_325
 */ 
class MutatorFrontendAction_325 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(325)

private:
    class MutatorASTConsumer_325 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_325(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> candidateFunctions;
    };
};

//source file
#include "../include/Insert_Static_Variable_In_Consteval_Function_325.h"

// ========================================================================================================
#define MUT325_OUTPUT 1

void MutatorFrontendAction_325::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstevalFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if function is consteval and has a body
      if (!FD->isConsteval() || !FD->hasBody())
        return;
      //Check if function already contains static variable
      bool hasStaticVar = false;
      Stmt* body = FD->getBody();
      if (body) {
        for (auto child : body->children()) {
          if (auto* declStmt = dyn_cast<DeclStmt>(child)) {
            for (auto decl : declStmt->decls()) {
              if (auto* varDecl = dyn_cast<VarDecl>(decl)) {
                if (varDecl->isStaticLocal()) {
                  hasStaticVar = true;
                  break;
                }
              }
            }
          }
        }
      }
      //Record candidate function if no static variable present
      if (!hasStaticVar) {
        candidateFunctions.push_back(FD);
      }
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("ConstevalCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocation()))
        return;
      //Check if call is in constant context (e.g., consteval constructor, constinit)
      //We'll mutate any call to a candidate consteval function
      const FunctionDecl* callee = CE->getDirectCallee();
      if (!callee) return;
      
      //Find if callee is in candidateFunctions
      for (auto* candidate : candidateFunctions) {
        if (candidate == callee) {
          //Get the source code text of target node
          auto funcBody = stringutils::rangetoStr(*(Result.SourceManager), candidate->getBody()->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          //Insert static variable at beginning of function body
          size_t bracePos = funcBody.find("{");
          if (bracePos != std::string::npos) {
            funcBody.insert(bracePos + 1, "\n/*mut325*/static int mut_static_var = 0;\n");
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(candidate->getBody()->getSourceRange()), funcBody);
          }
          //Remove from candidates after mutation
          candidateFunctions.erase(std::remove(candidateFunctions.begin(), candidateFunctions.end(), candidate), candidateFunctions.end());
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_325::MutatorASTConsumer_325::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher funcMatcher = functionDecl(isConsteval()).bind("ConstevalFunction");
    StatementMatcher callMatcher = callExpr(callee(functionDecl(isConsteval()))).bind("ConstevalCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}