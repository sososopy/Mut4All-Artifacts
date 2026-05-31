//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Templates_440
 */ 
class MutatorFrontendAction_440 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(440)

private:
    class MutatorASTConsumer_440 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_440(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> templateClasses;
    };
};

//source file
#include "../include/insert_recursive_templates_440.h"

// ========================================================================================================
#define MUT440_OUTPUT 1

void MutatorFrontendAction_440::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition() || RD->isLambda())
            return;
        
        auto className = RD->getNameAsString();
        if (className.empty())
            return;

        templateClasses.push_back(RD);

        // Perform mutation on the source code text by applying string replacement
        std::string templateDef = "template<int N> struct RecursiveTemplate_" + className + " {\n";
        templateDef += "    static const int value = N * RecursiveTemplate_" + className + "<N-1>::value;\n";
        templateDef += "};\n";
        templateDef += "template<> struct RecursiveTemplate_" + className + "<0> {\n";
        templateDef += "    static const int value = 1;\n";
        templateDef += "};\n";
        templateDef = "/*mut440*/" + templateDef;

        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(RD->getEndLoc(), "\n" + templateDef);
    }
}

void MutatorFrontendAction_440::MutatorASTConsumer_440::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl(isClass()).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}