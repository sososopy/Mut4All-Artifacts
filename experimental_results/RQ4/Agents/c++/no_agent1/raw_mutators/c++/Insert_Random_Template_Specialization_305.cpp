//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Random_Template_Specialization_305
 */ 
class MutatorFrontendAction_305 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(305)
private:
    class MutatorASTConsumer_305 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_305(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> visitedTemplates;
    };
};

//source file
#include "../include/insert_random_template_specialization_305.h"

// ========================================================================================================
#define MUT305_OUTPUT 1

void MutatorFrontendAction_305::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        
        std::string templateName = TD->getNameAsString();
        if (visitedTemplates.find(templateName) != visitedTemplates.end())
            return;

        visitedTemplates.insert(templateName);

        std::string specialization = "template<> class " + templateName + "<int> {\npublic:\n    void specializedFunction() {}\n};\n";
        specialization = "/*mut305*/" + specialization;

        SourceLocation insertLoc = TD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertTextAfter(insertLoc, specialization);
    }
}

void MutatorFrontendAction_305::MutatorASTConsumer_305::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}