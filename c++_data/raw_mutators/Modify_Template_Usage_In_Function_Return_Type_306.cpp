//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Usage_In_Function_Return_Type_306
 */ 
class MutatorFrontendAction_306 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(306)

private:
    class MutatorASTConsumer_306 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_306(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_usage_in_function_return_type_306.h"

// ========================================================================================================
#define MUT306_OUTPUT 1

void MutatorFrontendAction_306::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithAutoReturn")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getReturnType()->isUndeducedAutoType()) {
            if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TemplateAlias")) {
                auto aliasSourceRange = TA->getSourceRange();
                auto aliasText = stringutils::rangetoStr(*(Result.SourceManager), aliasSourceRange);

                // Perform mutation by adding an incompatible type to the alias
                auto modifiedAliasText = aliasText;
                size_t pos = modifiedAliasText.find_last_of('>');
                if (pos != std::string::npos) {
                    modifiedAliasText.insert(pos, ", void");
                }

                // Replace the original alias with the modified one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(aliasSourceRange), modifiedAliasText);
            }
        }
    }
}

void MutatorFrontendAction_306::MutatorASTConsumer_306::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto functionMatcher = functionDecl(returns(autoType())).bind("FunctionWithAutoReturn");
    auto aliasMatcher = typeAliasDecl().bind("TemplateAlias");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(aliasMatcher, &callback);
    matchFinder.matchAST(Context);
}