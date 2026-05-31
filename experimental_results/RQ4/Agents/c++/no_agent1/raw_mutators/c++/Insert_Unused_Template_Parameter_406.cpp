//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Parameter_406
 */ 
class MutatorFrontendAction_406 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(406)

private:
    class MutatorASTConsumer_406 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_406(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl*> visitedClasses;
    };
};

//source file
#include "../include/insert_unused_template_parameter_406.h"

// ========================================================================================================
#define MUT406_OUTPUT 1

void MutatorFrontendAction_406::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CD->getLocation()))
            return;
        if (!CD->isCompleteDefinition() || CD->isLambda())
            return;
        
        if (visitedClasses.find(CD) != visitedClasses.end())
            return;
        
        visitedClasses.insert(CD);
        
        // Get the source code text of target node
        auto className = CD->getNameAsString();
        if (className.empty())
            return;

        // Perform mutation on the source code text by applying string replacement
        std::string templateParam = "template<typename UnusedParam>\n";
        std::string classDef = stringutils::rangetoStr(*(Result.SourceManager), CD->getSourceRange());
        classDef.insert(0, templateParam);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), classDef);
    }
}

void MutatorFrontendAction_406::MutatorASTConsumer_406::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(unless(isTemplateInstantiation())).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}