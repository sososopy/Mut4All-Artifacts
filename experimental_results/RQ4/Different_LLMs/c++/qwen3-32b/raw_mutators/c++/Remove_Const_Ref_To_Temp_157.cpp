//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Const_Ref_To_Temp_157
 */ 
class MutatorFrontendAction_157 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(157)

private:
    class MutatorASTConsumer_157 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_157(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Remove_Const_Ref_To_Temp_157.h"

// ========================================================================================================
#define MUT157_OUTPUT 1

void MutatorFrontendAction_157::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        // Check if the initializer is a CallExpr (assuming it returns a temporary)
        if (auto *Init = VD->getInit()) {
            if (!isa<clang::CallExpr>(Init)) {
                return;
            }
        }

        // Get the type source range
        const clang::TypeLoc typeLoc = VD->getTypeSourceInfo()->getTypeLoc();
        clang::SourceRange typeRange = typeLoc.getSourceRange();

        // Get the type text
        std::string typeText = stringutils::rangetoStr(*Result.SourceManager, typeRange);

        // Remove 'const' from the type
        if (typeText.find("const ") == 0) {
            typeText = typeText.substr(6); // Remove 'const '
        } else if (typeText.find("const") == 0) {
            typeText = typeText.substr(5); // Remove 'const'
        }

        // Replace the type in the source code
        Rewrite.ReplaceText(typeRange, typeText);
    }
}
  
void MutatorFrontendAction_157::MutatorASTConsumer_157::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(
        hasType(referenceType(pointee(isConstQualified()))),
        hasInitializer(callExpr()),
        isWrittenInMainFile()
    ).bind("var");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}