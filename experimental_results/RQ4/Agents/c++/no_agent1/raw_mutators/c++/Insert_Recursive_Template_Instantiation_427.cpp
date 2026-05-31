//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_427
 */ 
class MutatorFrontendAction_427 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(427)

private:
    class MutatorASTConsumer_427 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_427(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_427.h"

// ========================================================================================================
#define MUT427_OUTPUT 1

void MutatorFrontendAction_427::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;
        
        if (visitedTemplates.find(TD->getTemplatedDecl()) != visitedTemplates.end())
            return;

        visitedTemplates.insert(TD->getTemplatedDecl());

        //Get the source code text of target node
        auto templateName = TD->getNameAsString();
        std::string instantiationCode = "\n/*mut427*/template<> class " + templateName + "<" + templateName + "<int>> {};\n";
        
        //Perform mutation on the source code text by applying string replacement
        Rewrite.InsertTextAfterToken(TD->getEndLoc(), instantiationCode);
    }
}

void MutatorFrontendAction_427::MutatorASTConsumer_427::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}