//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_474
 */ 
class MutatorFrontendAction_474 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(474)

private:
    class MutatorASTConsumer_474 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_474(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> template_classes;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_474.h"

// ========================================================================================================
#define MUT474_OUTPUT 1

void MutatorFrontendAction_474::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClasses")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if (!CL->isCompleteDefinition() || !CL->isTemplateDecl())
            return;
        template_classes.push_back(CL);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (template_classes.empty())
            return;
        
        auto target = template_classes.back();
        std::string template_name = target->getNameAsString();
        std::string recursive_instantiation = "/*mut474*/" + template_name + "<" + template_name + "<int>> obj;";
        
        if (FD->hasBody()) {
            auto body = FD->getBody();
            SourceLocation insertLoc = body->getBeginLoc().getLocWithOffset(1);
            Rewrite.InsertText(insertLoc, recursive_instantiation + "\n");
        }
    }
}

void MutatorFrontendAction_474::MutatorASTConsumer_474::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher template_matcher = cxxRecordDecl(isTemplateDecl()).bind("TemplateClasses");
    DeclarationMatcher function_matcher = functionDecl(hasBody()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.matchAST(Context);
}