//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Specialization_19
 */ 
class MutatorFrontendAction_19 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(19)

private:
    class MutatorASTConsumer_19 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_19(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Specialization_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
        if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(CTSD->getLocation()))
            return;
        
        const ClassTemplateDecl *CTD = CTSD->getSpecializedTemplate();
        if (!CTD)
            return;

        std::string templateName = CTD->getNameAsString();
        std::string newSpecialization = "template<typename T1, typename T2>\nclass " + templateName + "<T1, T2> {};\n";
        SourceLocation insertLoc = CTD->getEndLoc().getLocWithOffset(1);

        Rewrite.InsertText(insertLoc, "/*mut19*/\n" + newSpecialization);
    }
}

void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}