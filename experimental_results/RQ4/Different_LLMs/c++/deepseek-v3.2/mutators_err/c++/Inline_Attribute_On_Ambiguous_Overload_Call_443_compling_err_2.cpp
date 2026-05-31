//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Attribute_On_Ambiguous_Overload_Call_443
 */ 
class MutatorFrontendAction_443 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(443)

private:
    class MutatorASTConsumer_443 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_443(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Inline_Attribute_On_Ambiguous_Overload_Call_443.h"

// ========================================================================================================
#define MUT443_OUTPUT 1

void MutatorFrontendAction_443::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Check if the call is inside a template function
      auto *ParentFunc = Result.Context->getParents(*CE)[0].get<clang::FunctionDecl>();
      if (!ParentFunc || !ParentFunc->isTemplated())
        return;
      
      // Check if the called function name refers to an overload set
      auto *Callee = CE->getCalleeDecl();
      if (!Callee)
        return;
      
      // Check if there are multiple overloads for this function name
      auto *NamedDecl = dyn_cast<clang::NamedDecl>(Callee);
      if (!NamedDecl)
        return;
      
      // Look for other declarations with the same name in the same scope
      auto Name = NamedDecl->getName();
      auto *Scope = NamedDecl->getDeclContext();
      int OverloadCount = 0;
      for (auto *Decl : Scope->decls()) {
        if (auto *OtherNamed = dyn_cast<clang::NamedDecl>(Decl)) {
          if (OtherNamed->getName() == Name) {
            OverloadCount++;
          }
        }
      }
      if (OverloadCount < 2)
        return;
      
      // Check if the call already has an inline attribute
      auto SourceRange = CE->getSourceRange();
      auto SourceText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      if (SourceText.find("[[clang::always_inline]]") != std::string::npos ||
          SourceText.find("[[gnu::always_inline]]") != std::string::npos ||
          SourceText.find("__attribute__((always_inline))") != std::string::npos)
        return;
      
      //Get the source code text of target node
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText = "[[clang::always_inline]] " + SourceText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
    }
}
  
void MutatorFrontendAction_443::MutatorASTConsumer_443::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = callExpr(hasParent(functionDecl(isTemplated()))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}