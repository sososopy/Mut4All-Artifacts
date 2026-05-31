//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateLambdaNestedReturn_45
 */ 
class MutatorFrontendAction_45 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(45)

private:
    class MutatorASTConsumer_45 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_45(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_TemplateLambdaNestedReturn_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Check if the function has at least one parameter
        if (FD->getNumParams() == 0)
            return;

        // Replace return type
        SourceLocation returnTypeLoc = FD->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
        SourceLocation returnTypeEndLoc = FD->getTypeSourceInfo()->getTypeLoc().getEndLoc();
        Rewrite.ReplaceText(SourceRange(returnTypeLoc, returnTypeEndLoc), "auto");

        // Get first parameter
        const clang::ParmVarDecl *firstParam = FD->getParamDecl(0);
        std::string firstParamType = firstParam->getType().getAsString();
        std::string firstParamName = firstParam->getNameAsString();

        // Generate nested return type
        std::string nestedReturnType = "typename std::vector<typename std::map<" + firstParamType + ", " + firstParamType + ">::value_type>::size_type";

        // Generate lambda code
        std::string lambdaCode = "return [" + firstParamName + "](" + firstParamType + " y) -> " + nestedReturnType + " { return y.size(); }(" + firstParamName + ");";

        // Replace function body
        clang::CompoundStmt *body = FD->getBody();
        if (!body)
            return;

        SourceLocation bodyStart = body->getLBracLoc();
        SourceLocation bodyEnd = body->getRBracLoc();
        Rewrite.ReplaceText(SourceRange(bodyStart, bodyEnd), lambdaCode);
    }
}

void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isFunctionTemplate()).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}