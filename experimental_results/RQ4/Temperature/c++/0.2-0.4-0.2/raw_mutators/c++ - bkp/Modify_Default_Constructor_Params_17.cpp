//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_default_constructor_params_17
 */ 
class MutatorFrontendAction_17 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(17)

private:
    class MutatorASTConsumer_17 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_17(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_default_constructor_params_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CtorDecl = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("CtorWithDefaults")) {
        if (!CtorDecl || !Result.Context->getSourceManager().isWrittenInMainFile(CtorDecl->getLocation()))
            return;

        bool hasDefaultParam = false;
        for (const auto *param : CtorDecl->parameters()) {
            if (param->hasDefaultArg()) {
                hasDefaultParam = true;
                break;
            }
        }

        if (hasDefaultParam) {
            auto ctorSourceRange = CtorDecl->getSourceRange();
            std::string ctorSource = stringutils::rangetoStr(*(Result.SourceManager), ctorSourceRange);
            
            for (const auto *param : CtorDecl->parameters()) {
                if (param->hasDefaultArg()) {
                    std::string paramName = param->getNameAsString();
                    size_t pos = ctorSource.find("=" + param->getDefaultArg()->getStmtClassName());
                    if (pos != std::string::npos) {
                        size_t endPos = ctorSource.find(",", pos);
                        if (endPos == std::string::npos) {
                            endPos = ctorSource.find(")", pos);
                        }
                        ctorSource.erase(pos, endPos - pos);
                        break;
                    }
                }
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(ctorSourceRange), ctorSource);
        }
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(hasAnyParameter(hasDefaultArgument())).bind("CtorWithDefaults");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}