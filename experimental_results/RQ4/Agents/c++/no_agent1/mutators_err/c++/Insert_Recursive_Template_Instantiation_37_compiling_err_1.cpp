//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_37
 */ 
class MutatorFrontendAction_37 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(37)

private:
    class MutatorASTConsumer_37 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_37(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> functionDecls;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                FD->getLocation()))
            return;

        if (FD->hasBody() && FD->isTemplateInstantiation()) {
            functionDecls.push_back(FD);
        }
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                TD->getLocation()))
            return;
        
        if (!functionDecls.empty()) {
            auto *FD = functionDecls.back();
            functionDecls.pop_back();
            
            std::string templateText = "template<typename T> struct RecursiveTemplate { using Type = typename RecursiveTemplate<T>::Type; };\n";
            std::string instantiationText = "RecursiveTemplate<int>::Type instance;";
            
            SourceLocation insertLoc = FD->getBody()->getBeginLoc();
            Rewrite.InsertTextBefore(insertLoc, "/*mut37*/" + templateText + instantiationText);
        }
    }
}

void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl(isTemplateInstantiation()).bind("Functions");
    DeclarationMatcher templateMatcher = templateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}