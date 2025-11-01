//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Attribute_With_Overloaded_Functions_443
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
        std::vector<const clang::FunctionDecl *> overloadedFunctions;
    };
};

//source file
#include "../include/inline_attribute_with_overloaded_functions_443.h"

// ========================================================================================================
#define MUT443_OUTPUT 1

void MutatorFrontendAction_443::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("OverloadedFunc")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isOverloadedOperator()) {
        overloadedFunctions.push_back(FD);
        return;
      }

      if (FD->hasBody() && !FD->hasAttr<clang::AlwaysInlineAttr>()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        if (!overloadedFunctions.empty()) {
          declaration.insert(declaration.find("{") + 1, "\n[[clang::always_inline]] ");
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_443::MutatorASTConsumer_443::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasOverloadedOperatorName()).bind("OverloadedFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}