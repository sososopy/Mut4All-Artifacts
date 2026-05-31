//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Syntax_Error_And_Dependent_Type_340
 */ 
class MutatorFrontendAction_340 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(340)

private:
    class MutatorASTConsumer_340 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_340(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Syntax_Error_And_Dependent_Type_340.h"

// ========================================================================================================
#define MUT340_OUTPUT 1

void MutatorFrontendAction_340::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        // Filter nodes in header files
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;

        // Get the source code of the class
        auto SM = Result.SourceManager;
        auto classRange = CL->getSourceRange();
        auto classCode = stringutils::rangetoStr(*SM, classRange);

        // Find the position of the opening brace
        size_t openBracePos = classCode.find('{');
        if (openBracePos == string::npos)
            return;

        // Insert the syntax error, static function, and dependent type member
        string insertText = "\n  p; // syntax error\n  static void foo() {};\n  C<int, decltype(foo)> c;";
        classCode.insert(openBracePos + 1, insertText);

        // Replace the original class code with the modified code
        Rewrite.ReplaceText(classRange, classCode);
    }
}

void MutatorFrontendAction_340::MutatorASTConsumer_340::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}