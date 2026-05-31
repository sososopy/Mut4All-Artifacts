//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Overloaded_Comma_Operators_236
 */ 
class MutatorFrontendAction_236 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(236)

private:
    class MutatorASTConsumer_236 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_236(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *CurrentStruct = nullptr;
    };
};

//source file
#include "../include/modify_overloaded_comma_operators_236.h"

// ========================================================================================================
#define MUT236_OUTPUT 1

void MutatorFrontendAction_236::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *StructDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructWithComma")) {
        //Filter nodes in header files
        if (!StructDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                StructDecl->getLocation()))
            return;

        CurrentStruct = StructDecl;

        //Get the source code text of target node
        auto structText = stringutils::rangetoStr(*(Result.SourceManager), StructDecl->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string newOverloads = "\nfriend void operator,(Container, char);\n"
                                   "friend void operator,(Container, Container*);\n"
                                   "friend void operator,(Container, ...);\n";

        //Insert new overloads before the closing brace of the struct
        if (structText.rfind('}') != std::string::npos)
            structText.insert(structText.rfind('}'), newOverloads);

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(StructDecl->getSourceRange()), structText);
    }
}

void MutatorFrontendAction_236::MutatorASTConsumer_236::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasMethod(cxxMethodDecl(isOverloadedOperator()).bind("CommaOperator"))).bind("StructWithComma");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}