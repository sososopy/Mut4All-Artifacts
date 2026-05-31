//header file
#pragma once
#include "Mutator_base.h"

/**
 * Simplify_Lambda_Derived_Return_22
 */ 
class MutatorFrontendAction_22 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(22)

private:
    class MutatorASTConsumer_22 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_22(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Simplify_Lambda_Derived_Return_22.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("targetFunc")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      auto *FTD = MT;
      auto *TemplatedFunc = FTD->getTemplatedDecl();
      if (!TemplatedFunc || !isa<CXXMethodDecl>(TemplatedFunc))
        return;
      
      auto *Method = cast<CXXMethodDecl>(TemplatedFunc);
      if (!Method->hasBody())
        return;
      
      // Check for trailing return type with decltype
      auto *TrailingReturn = Method->getReturnType().getTypePtrOrNull();
      if (!TrailingReturn || TrailingReturn->getTypeClass() != Type::Decltype)
        return;
      
      // Check if decltype contains a call to base lambda operator
      auto *DT = dyn_cast<DecltypeType>(TrailingReturn);
      if (!DT)
        return;
      
      auto *Expr = DT->getUnderlyingExpr();
      if (!Expr)
       2return;
      
      // Look for call to Func::operator() in the decltype expression
      bool HasBaseCall = false;
      auto CheckForBaseCall = [&](const Stmt *S) -> bool {
        if (!S) return false;
        if (auto *CE = dyn_cast<CXXMemberCallExpr>(S)) {
          if (auto *ME = CE->getMethodDecl()) {
            if (ME->getNameAsString() == "operator()") {
              // Check if calling base class operator
              auto *Base = dyn_cast<CXXRecordDecl>(ME->getParent());
              if (Base && Base->getQualifiedNameAsString() == "Func") {
                HasBaseCall = true;
                return true;
              }
            }
          }
        }
        return false;
      };
      
      // Traverse the expression to find base call
      std::function<void(const Stmt*)> Traverse = [&](const Stmt *S) {
        if (!S || HasBaseCall) return;
        if (CheckForBaseCall(S)) return;
        for (auto *Child : S->children()) {
          Traverse(Child);
          if (HasBaseCall) return;
        }
      };
      
      Traverse(Expr);
      if (!HasBaseCall)
        return;
      
      // Get the full function source range
      SourceRange FuncRange = Method->getSourceRange();
      if (FuncRange.isInvalid())
        return;
      
      // Get function source code
      auto FuncText = stringutils::rangetoStr(*(Result.SourceManager), FuncRange);
      
      // Find the position of trailing return type
      size_t TrailingPos = FuncText.find("->");
      if (TrailingPos == std::string::npos)
        return;
      
      // Find the opening brace of function body
      size_t BodyPos = FuncText.find("{", TrailingPos);
      if (BodyPos == std::string::npos)
        return;
      
      // Extract everything before trailing return type
      std::string Prefix = FuncText.substr(0, TrailingPos);
      
      // Extract function body (from { to })
      size_t BodyEnd = FuncText.rfind("}");
      if (BodyEnd == std::string::npos)
        return;
      
      std::string Body = FuncText.substr(BodyPos, BodyEnd - BodyPos + 1);
      
      // Construct new function without trailing return type
      std::string NewFunc = Prefix + Body;
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FuncRange), NewFunc);
    }
}
  
void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(
      hasAncestor(cxxRecordDecl(isDerivedFrom(hasName("Func")))),
      has(templatedDecl(
        cxxMethodDecl(
          hasTrailingReturn(),
          hasParent(cxxRecordDecl(isDerivedFrom(hasName("Func"))))
        )
      ))
    ).bind("targetFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}