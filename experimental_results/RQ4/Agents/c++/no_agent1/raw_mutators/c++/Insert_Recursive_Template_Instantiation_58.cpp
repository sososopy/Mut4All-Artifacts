//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_58
 */ 

class MutatorFrontendAction_58 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(58)
private:
    class MutatorASTConsumer_58 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_58(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::ClassTemplateDecl *> visitedTemplates;
    };
};

//source file
#include "../include/insert_recursive_template_instantiation_58.h"

// ========================================================================================================
#define MUT58_OUTPUT 1

void MutatorFrontendAction_58::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;
        
        if (visitedTemplates.find(CTD) != visitedTemplates.end())
            return;

        visitedTemplates.insert(CTD);

        // Get the source code text of target node
        auto templateName = CTD->getNameAsString();
        
        // Perform mutation on the source code text by applying string replacement
        std::string recursiveInstantiation = "template<> struct " + templateName + "<0> { enum { value = 0 }; };\n";
        recursiveInstantiation += "template<int N> struct " + templateName + " { enum { value = " + templateName + "<N-1>::value + 1 }; };\n";
        recursiveInstantiation = "/*mut58*/" + recursiveInstantiation;

        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(CTD->getEndLoc(), recursiveInstantiation);
    }
}

void MutatorFrontendAction_58::MutatorASTConsumer_58::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}