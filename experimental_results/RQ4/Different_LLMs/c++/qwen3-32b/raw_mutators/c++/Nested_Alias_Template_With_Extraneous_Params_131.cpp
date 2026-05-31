//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"

/**
 * Nested_Alias_Template_With_Extraneous_Params_131
 */ 
class MutatorFrontendAction_131 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(131)

private:
    class MutatorASTConsumer_131 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_131(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Nested_Alias_Template_With_Extraneous_Params_131.h"

// ========================================================================================================
#define MUT131_OUTPUT 1

void MutatorFrontendAction_131::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassTemplates")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        if (!CD->isClass() || !CD->isCompleteDefinition())
            return;
        TemplateParameterList *TPL = CD->getTemplateParameterList();
        if (!TPL || TPL->size() < 1)
            return;
        std::string className = CD->getNameAsString();
        TemplateParameter *firstParam = TPL->getParam(0);
        std::string outerParamName = firstParam->getDeclName().getAsString();
        std::string aliasParamName = "K";
        std::string aliasTemplate = "  template <typename " + aliasParamName + ">\n  using " + className + " = " + className + "<" + aliasParamName + ">;\n";
        SourceLocation insertLoc = CD->getBody()->getEndLoc();
        Rewrite.InsertText(insertLoc, aliasTemplate, true, true);
    }
}

void MutatorFrontendAction_131::MutatorASTConsumer_131::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isClass(), isCompleteDefinition(), hasTemplateParameters(anything())).bind("ClassTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}