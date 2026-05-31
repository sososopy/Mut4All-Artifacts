//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_166
 */ 
class MutatorFrontendAction_166 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(166)

private:
    class MutatorASTConsumer_166 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_166(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::ClassTemplateDecl *> processedTemplates;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_166.h"

// ========================================================================================================
#define MUT166_OUTPUT 1

void MutatorFrontendAction_166::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CTD->getLocation()))
            return;
        
        if (processedTemplates.find(CTD) != processedTemplates.end())
            return;

        processedTemplates.insert(CTD);

        auto templateName = CTD->getNameAsString();
        std::string recursiveInstantiation = "template<> class " + templateName + "<0> { public: static const int value = " + templateName + "<1>::value; };";
        std::string instantiation = "\n/*mut166*/" + recursiveInstantiation + "\n";

        SourceLocation insertLoc = CTD->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, instantiation);
    }
}

void MutatorFrontendAction_166::MutatorASTConsumer_166::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}