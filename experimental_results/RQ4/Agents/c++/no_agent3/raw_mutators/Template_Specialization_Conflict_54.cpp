//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Specialization_Conflict_54
 */ 
class MutatorFrontendAction_54 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(54)

private:
    class MutatorASTConsumer_54 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_54(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const TemplateDecl*> visitedTemplates;
    };
};

//source file
#include "../include/Template_Specialization_Conflict_54.h"

// ========================================================================================================
#define MUT54_OUTPUT 1

void MutatorFrontendAction_54::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (visitedTemplates.find(TD->getSpecializedTemplate()) != visitedTemplates.end())
            return;

        visitedTemplates.insert(TD->getSpecializedTemplate());

        std::string templateName = TD->getSpecializedTemplate()->getNameAsString();
        std::string newSpec = "template<> class " + templateName + "<int, double> { public: void doSomething(); };";
        SourceLocation insertLoc = TD->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut54*/" + newSpec);
    }
}
  
void MutatorFrontendAction_54::MutatorASTConsumer_54::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}