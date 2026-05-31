//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Parameter_343
 */ 
class MutatorFrontendAction_343 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(343)

private:
    class MutatorASTConsumer_343 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_343(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl *> processedClasses;
    };
};

//source file
#include "../include/insert_unused_template_parameter_343.h"

// ========================================================================================================
#define MUT343_OUTPUT 1

void MutatorFrontendAction_343::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (RD->isCompleteDefinition() == false || processedClasses.count(RD))
            return;

        processedClasses.insert(RD);

        // Get the source code text of target node
        std::string classText = stringutils::rangetoStr(*(Result.SourceManager), RD->getSourceRange());
        
        // Perform mutation on the source code text by applying string replacement
        std::string templateParam = "template<typename UnusedParam> ";
        classText.insert(0, templateParam);
        
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()), classText);
    }
}

void MutatorFrontendAction_343::MutatorASTConsumer_343::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}