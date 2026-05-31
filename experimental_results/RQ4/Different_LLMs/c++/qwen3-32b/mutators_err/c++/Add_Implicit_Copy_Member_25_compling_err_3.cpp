//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Implicit_Copy_Member_25
 */ 
class MutatorFrontendAction_25 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(25)

private:
    class MutatorASTConsumer_25 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_25(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_Implicit_Copy_Member_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("class")) {
        // Filter nodes in header files
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
            return;

        // Skip if user-declared copy constructor or assignment exists
        if (CL->hasUserDeclaredCopyConstructor() || CL->hasUserDeclaredCopyAssignment()) {
            return;
        }

        // Get insertion location
        SourceLocation endLoc = CL->getEndLoc();
        if (endLoc.isInvalid()) {
            return;
        }

        // Generate default copy constructor
        std::string className = CL->getNameAsString();
        std::string insertText = "\n    " + className + "(const " + className + "&) = default;\n";

        // Insert before closing brace
        Rewrite.InsertText(endLoc, insertText);
    }
}

void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Match classes with template constructors and no user-declared copy members
    DeclarationMatcher matcher = cxxRecordDecl(
        has(cxxConstructorDecl(clang::ast_matchers::isTemplateInstantiation())),
        bind("class")
    );
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}