//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_316
 */ 
class MutatorFrontendAction_316 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(316)

private:
    class MutatorASTConsumer_316 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_316(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl *> visitedTemplates;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_316.h"

// ========================================================================================================
#define MUT316_OUTPUT 1

void MutatorFrontendAction_316::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;
        
        if (visitedTemplates.find(TD->getTemplatedDecl()) != visitedTemplates.end())
            return;

        visitedTemplates.insert(TD->getTemplatedDecl());

        // Get the source code text of target node
        std::string templateName = TD->getNameAsString();
        std::string recursiveInstantiation = "\n/*mut316*/template<> struct " + templateName + "<0> { using type = " + templateName + "<0>; };";

        // Perform mutation on the source code text by applying string replacement
        SourceLocation insertLoc = TD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertTextAfter(insertLoc, recursiveInstantiation);
    }
}

void MutatorFrontendAction_316::MutatorASTConsumer_316::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}