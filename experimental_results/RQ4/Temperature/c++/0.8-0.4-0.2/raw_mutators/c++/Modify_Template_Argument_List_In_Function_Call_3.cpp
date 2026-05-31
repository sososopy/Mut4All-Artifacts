//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Argument_List_In_Function_Call_3
 */ 
class MutatorFrontendAction_3 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(3)

private:
    class MutatorASTConsumer_3 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_3(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_argument_list_in_function_call_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                CallExpr->getExprLoc()))
            return;

        if (auto *TemplateSpecialization = dyn_cast<clang::FunctionDecl>(CallExpr->getCalleeDecl())) {
            if (TemplateSpecialization->getTemplateSpecializationArgs()) {
                auto *TemplateArgs = TemplateSpecialization->getTemplateSpecializationArgs()->asArray();
                std::string modifiedArgs;

                for (unsigned i = 0; i < TemplateArgs.size(); ++i) {
                    if (i == 1) {
                        modifiedArgs += "std::string";
                    } else {
                        modifiedArgs += TemplateArgs[i].getAsType().getAsString();
                    }
                    if (i < TemplateArgs.size() - 1) {
                        modifiedArgs += ", ";
                    }
                }

                std::string originalText = clang::Lexer::getSourceText(
                    clang::CharSourceRange::getTokenRange(CallExpr->getSourceRange()),
                    Result.Context->getSourceManager(), Result.Context->getLangOpts()).str();

                size_t angleBracketPos = originalText.find('<');
                size_t closingBracketPos = originalText.find('>');

                if (angleBracketPos != std::string::npos && closingBracketPos != std::string::npos) {
                    std::string mutatedText = originalText.substr(0, angleBracketPos + 1) +
                                              modifiedArgs +
                                              originalText.substr(closingBracketPos);
                    Rewrite.ReplaceText(CallExpr->getSourceRange(), mutatedText);
                }
            }
        }
    }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("TemplateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}