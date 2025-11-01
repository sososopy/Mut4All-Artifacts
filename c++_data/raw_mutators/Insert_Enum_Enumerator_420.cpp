//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Enum_Enumerator_420
 */ 
class MutatorFrontendAction_420 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(420)

private:
    class MutatorASTConsumer_420 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_420(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Enum_Enumerator_420.h"

// ========================================================================================================
#define MUT420_OUTPUT 1

void MutatorFrontendAction_420::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        //Filter nodes in header files
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ED->getLocation()))
            return;
        
        //Get the source code text of target node
        auto enumSource = stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        std::string newEnumerator = "NEW_ENUMERATOR"; // Name should follow existing naming conventions
        std::string insertion = ", " + newEnumerator;

        // Find the position to insert the new enumerator
        size_t insertPos = enumSource.rfind('}');
        if (insertPos != std::string::npos) {
            enumSource.insert(insertPos, insertion);
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), enumSource);
    }
}

void MutatorFrontendAction_420::MutatorASTConsumer_420::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}