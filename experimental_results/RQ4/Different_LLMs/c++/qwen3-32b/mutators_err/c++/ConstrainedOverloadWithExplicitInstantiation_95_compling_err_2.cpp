//header file
#pragma once
#include "Mutator_base.h"

/**
 * ConstrainedOverloadWithExplicitInstantiation_95
 */ 
class MutatorFrontendAction_95 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(95)

private:
    class MutatorASTConsumer_95 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_95(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::ConceptDecl *> concepts;
    };
};

//source file
#include "../include/ConstrainedOverloadWithExplicitInstantiation_95.h"

// ========================================================================================================
#define MUT95_OUTPUT 1

void MutatorFrontendAction_95::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("concept")) {
        concepts.push_back(CD);
        return;
    }

    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;

        if (concepts.empty())
            return;

        auto *ClassTemplate = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("classTemplate");
        if (!ClassTemplate)
            return;

        std::string className = ClassTemplate->getNameAsString();
        if (className.empty())
            return;

        auto *TemplateParams = ClassTemplate->getTemplateParameters();
        if (!TemplateParams || TemplateParams->size() == 0)
            return;
        std::string paramName = TemplateParams->getParam(0)->getNameAsString();
        if (paramName.empty())
            return;

        std::string methodName = MD->getNameAsString();
        if (methodName.empty())
            return;

        auto *Concept = concepts[0];
        std::string conceptName = Concept->getNameAsString();
        if (conceptName.empty())
            return;

        std::string newMethodCode = "    void " + methodName + "() requires " + conceptName + "<" + paramName + ">();\n";
        SourceLocation methodEnd = MD->getSourceRange().getEnd();
        Rewrite.InsertTextAfter(methodEnd, newMethodCode);

        std::string explicitInstantiation = "template void " + className + "<int>::" + methodName + "();\n";
        SourceLocation classEnd = ClassTemplate->getSourceRange().getEnd();
        Rewrite.InsertTextAfter(classEnd, "/*mut95*/" + explicitInstantiation);
    }
}

void MutatorFrontendAction_95::MutatorASTConsumer_95::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher methodMatcher = cxxMethodDecl(
        hasParent(classTemplateDecl().bind("classTemplate"))
    ).bind("method");
    DeclarationMatcher conceptMatcher = clang::ast_matchers::conceptDecl().bind("concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.matchAST(Context);
}