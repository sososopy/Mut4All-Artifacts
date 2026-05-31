//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_specialization_incomplete_type_690
 */ 
class MutatorFrontendAction_690 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(690)

private:
    class MutatorASTConsumer_690 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_690(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_specialization_incomplete_type_690.h"

// ========================================================================================================
#define MUT690_OUTPUT 1

void MutatorFrontendAction_690::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto templateName = TD->getNameAsString();
        auto specialization = "class UndefinedType; // Forward declaration only\n";
        specialization += "template<> class " + templateName + "<UndefinedType>;\n";
        
        SourceLocation insertLoc = TD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, "\n/*mut690*/" + specialization);
    }
}

void MutatorFrontendAction_690::MutatorASTConsumer_690::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}