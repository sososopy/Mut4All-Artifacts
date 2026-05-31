//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Template_Instantiation_388
 */ 

class MutatorFrontendAction_388 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(388)
private:
    class MutatorASTConsumer_388 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_388(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> functions;
    };
};

//source file
#include "../include/insert_template_instantiation_388.h"

// ========================================================================================================
#define MUT388_OUTPUT 1

void MutatorFrontendAction_388::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (FD->isTemplated())
            return;
        functions.push_back(FD);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;
        
        if (functions.empty())
            return;

        auto templateName = TD->getNameAsString();
        std::string instantiation = "/*mut388*/ " + templateName + "<int> instance;\n";
        auto *function = functions.back();
        functions.pop_back();

        auto functionSourceRange = function->getSourceRange();
        auto functionSourceText = stringutils::rangetoStr(*(Result.SourceManager), functionSourceRange);
        functionSourceText.insert(functionSourceText.find("{") + 1, instantiation);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(functionSourceRange), functionSourceText);
    }
}
  
void MutatorFrontendAction_388::MutatorASTConsumer_388::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl().bind("Functions");
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}