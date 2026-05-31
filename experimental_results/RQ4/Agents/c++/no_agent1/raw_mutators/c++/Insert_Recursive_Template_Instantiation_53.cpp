//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_53
 */ 
class MutatorFrontendAction_53 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(53)

private:
    class MutatorASTConsumer_53 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_53(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::TemplateDecl *> visitedTemplates;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (visitedTemplates.find(TD) != visitedTemplates.end())
            return;

        visitedTemplates.insert(TD);

        auto templateName = TD->getNameAsString();
        if (templateName.empty())
            return;

        std::string recursiveInstantiation = "template<> struct " + templateName + "<0> { enum { value = " + templateName + "<0>::value }; };";
        recursiveInstantiation = "\n/*mut53*/" + recursiveInstantiation;

        Rewrite.InsertTextAfterToken(TD->getEndLoc(), recursiveInstantiation);
    }
}
  
void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}