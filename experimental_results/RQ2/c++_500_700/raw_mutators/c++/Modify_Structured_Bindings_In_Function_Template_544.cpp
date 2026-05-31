//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_structured_bindings_in_function_template_544
 */ 
class MutatorFrontendAction_544 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(544)

private:
    class MutatorASTConsumer_544 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_544(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/modify_structured_bindings_in_function_template_544.h"

// ========================================================================================================
#define MUT544_OUTPUT 1

void MutatorFrontendAction_544::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("structuredBinding")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (const auto *DRE = dyn_cast<clang::DeclRefExpr>(VD->getInit())) {
        const auto *InitExpr = DRE->getDecl();
        if (InitExpr && InitExpr->getType()->isAggregateType()) {
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                     VD->getSourceRange());
          size_t pos = declaration.find("[...]");
          if (pos != std::string::npos) {
            declaration.replace(pos, 5, "[x0, x1]");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_544::MutatorASTConsumer_544::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl(hasType(autoType()), hasInitializer(declRefExpr())).bind("structuredBinding");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}