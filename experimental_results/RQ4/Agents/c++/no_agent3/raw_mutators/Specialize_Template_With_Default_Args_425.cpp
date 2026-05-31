//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Template_With_Default_Args_425
 */ 
class MutatorFrontendAction_425 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(425)

private:
    class MutatorASTConsumer_425 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_425(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Specialize_Template_With_Default_Args_425.h"

// ========================================================================================================
#define MUT425_OUTPUT 1

void MutatorFrontendAction_425::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        
        if (TD->getTemplateParameters()->size() == 1) {
            auto *param = TD->getTemplateParameters()->getParam(0);
            if (auto *typeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                if (!typeParam->hasDefaultArgument()) {
                    // Get the source code text of target node
                    auto templateDeclText = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
                    
                    // Perform mutation on the source code text by applying string replacement
                    std::string defaultArg = " = int";
                    std::string oldParamText = "typename " + typeParam->getNameAsString();
                    std::string newParamText = oldParamText + defaultArg;
                    size_t pos = templateDeclText.find(oldParamText);
                    if (pos != std::string::npos) {
                        templateDeclText.replace(pos, oldParamText.length(), newParamText);
                    }

                    // Add a specialization
                    std::string specializationText = "template class " + TD->getNameAsString() + "<int>;\n";
                    
                    // Replace the original AST node with the mutated one
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDeclText + "\n/*mut425*/" + specializationText);
                }
            }
        }
    }
}

void MutatorFrontendAction_425::MutatorASTConsumer_425::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}