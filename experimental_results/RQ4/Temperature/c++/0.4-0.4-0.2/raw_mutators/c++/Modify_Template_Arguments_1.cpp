//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_arguments_1
 */ 
class MutatorFrontendAction_1 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1)

private:
    class MutatorASTConsumer_1 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_1(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Arguments_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        //Filter nodes in header files
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                CallExpr->getExprLoc()))
            return;
        
        // Get the template arguments and function call arguments
        if (auto *TemplateArgs = CallExpr->getDirectCallee()->getTemplateSpecializationArgs()) {
            unsigned NumTemplateArgs = TemplateArgs->size();
            unsigned NumCallArgs = CallExpr->getNumArgs();
            
            // Ensure there is a mismatch between the number of template arguments and call arguments
            if (NumTemplateArgs != NumCallArgs) {
                // Get the source code text of target node
                auto CallExprText = stringutils::rangetoStr(*(Result.SourceManager),
                                                            CallExpr->getSourceRange());
                // Perform mutation on the source code text by applying string replacement
                // For simplicity, remove one template argument to create a mismatch
                std::string MutatedCallExprText = CallExprText;
                size_t pos = MutatedCallExprText.find('<');
                if (pos != std::string::npos) {
                    size_t endPos = MutatedCallExprText.find('>', pos);
                    if (endPos != std::string::npos && endPos > pos + 1) {
                        size_t commaPos = MutatedCallExprText.find(',', pos);
                        if (commaPos != std::string::npos && commaPos < endPos) {
                            MutatedCallExprText.erase(commaPos, endPos - commaPos);
                        }
                    }
                }
                // Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(CallExpr->getSourceRange()), MutatedCallExprText);
            }
        }
    }
}

void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}