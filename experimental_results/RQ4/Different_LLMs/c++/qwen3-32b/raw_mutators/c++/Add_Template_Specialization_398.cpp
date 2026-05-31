//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Template_Specialization_398
 */ 
class MutatorFrontendAction_398 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(398)

private:
    class MutatorASTConsumer_398 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_398(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Template_Specialization_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *classTemplate = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("nonTypeClassTemplate")) {
        if (!classTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(classTemplate->getLocation()))
            return;

        auto primaryTemplate = classTemplate->getTemplatedDecl();
        if (!primaryTemplate || !primaryTemplate->isRecord())
            return;

        auto methodDecl = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("nonTypeMethod");
        if (!methodDecl)
            return;

        std::string className = classTemplate->getNameAsString();
        std::string methodName = methodDecl->getNameAsString();

        std::string code = "template<> auto " + className + "<T>::" + methodName + "<5>() { return 5; }\n";
        code += "template struct " + className + "<int>;\n";

        auto loc = primaryTemplate->getEndLoc();
        Rewrite.InsertTextAfterToken(loc, code);
    } else if (auto *classTemplate = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("staticDefaultClassTemplate")) {
        if (!classTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(classTemplate->getLocation()))
            return;

        auto primaryTemplate = classTemplate->getTemplatedDecl();
        if (!primaryTemplate || !primaryTemplate->isRecord())
            return;

        auto methodDecl = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("staticDefaultMethod");
        if (!methodDecl)
            return;

        std::string className = classTemplate->getNameAsString();
        std::string methodName = methodDecl->getNameAsString();

        std::string code = "template<> void " + className + "<T>::" + methodName + "<int>() { }\n";
        code += "template struct " + className + "<char>;\n";

        auto loc = primaryTemplate->getEndLoc();
        Rewrite.InsertTextAfterToken(loc, code);
    }
}
  
void MutatorFrontendAction_398::MutatorASTConsumer_398::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = classTemplateDecl(
        has(cxxMethodDecl(
            isTemplate(),
            hasDescendant(nonTypeTemplateParmDecl())
        ).bind("nonTypeMethod"))
    ).bind("nonTypeClassTemplate");

    DeclarationMatcher matcher2 = classTemplateDecl(
        has(cxxMethodDecl(
            isStatic(),
            isTemplate(),
            hasDescendant(templateParamDecl(hasDefaultArgument()))
        ).bind("staticDefaultMethod"))
    ).bind("staticDefaultClassTemplate");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}