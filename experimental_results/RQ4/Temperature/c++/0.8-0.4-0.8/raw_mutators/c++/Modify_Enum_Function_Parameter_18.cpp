//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_enum_function_parameter_18
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)

private:
    class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_18(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_enum_function_parameter_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithEnumParam")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        for (unsigned i = 0; i < FD->getNumParams(); ++i) {
            auto *param = FD->getParamDecl(i);
            if (param->getType()->isEnumeralType()) {
                auto paramRange = param->getSourceRange();
                auto paramText = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
                auto mutatedParamText = paramText;
                size_t enumPos = mutatedParamText.find("enum");
                if (enumPos != std::string::npos) {
                    mutatedParamText.replace(enumPos, 4, "invalid");
                }
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramRange), mutatedParamText);
                break;
            }
        }
    }
}

void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasParameter(0, hasType(enumType()))).bind("FunctionWithEnumParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}