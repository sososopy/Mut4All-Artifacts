//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Parameter_238
 */ 
class MutatorFrontendAction_238 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(238)

private:
    class MutatorASTConsumer_238 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_238(Rewriter &R) : TheRewriter(R) {}
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
        std::unordered_set<const clang::CXXRecordDecl *> processedClasses;
    };
};

//source file
#include "../include/insert_unused_template_parameter_238.h"

// ========================================================================================================
#define MUT238_OUTPUT 1

void MutatorFrontendAction_238::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                CL->getLocation()))
            return;
        if (CL->isCompleteDefinition() == false || CL->isLambda() || processedClasses.count(CL))
            return;

        processedClasses.insert(CL);

        auto className = CL->getNameAsString();
        if (className.empty())
            return;

        // Perform mutation by adding an unused template parameter
        std::string templateParam = "template<typename UnusedParam>\n";
        std::string classDecl = stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
        classDecl = templateParam + classDecl;

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), classDecl);
    }
}

void MutatorFrontendAction_238::MutatorASTConsumer_238::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}