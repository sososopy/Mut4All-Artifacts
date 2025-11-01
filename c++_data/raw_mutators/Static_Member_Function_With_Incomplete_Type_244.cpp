//header file
#pragma once
#include "Mutator_base.h"

/**
 * Static_Member_Function_With_Incomplete_Type_244
 */ 
class MutatorFrontendAction_244 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(244)

private:
    class MutatorASTConsumer_244 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_244(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Static_Member_Function_With_Incomplete_Type_244.h"

// ========================================================================================================
#define MUT244_OUTPUT 1

void MutatorFrontendAction_244::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SR = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        //Filter nodes in header files
        if (!SR || !Result.Context->getSourceManager().isWrittenInMainFile(
                       SR->getLocation()))
            return;
        if (!SR->isCompleteDefinition())
            return;

        //Get the source code text of target node
        auto structText = stringutils::rangetoStr(*(Result.SourceManager), SR->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string mutation = "\nstatic IncompleteType myFunction();\n";
        if (structText.rfind('}') != std::string::npos)
            structText.insert(structText.rfind('}'), "/*mut244*/" + mutation);

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SR->getSourceRange()), structText);
    }
}

void MutatorFrontendAction_244::MutatorASTConsumer_244::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}