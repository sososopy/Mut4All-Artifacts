//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Argument_Parameter_Mismatch_382
 */ 
class MutatorFrontendAction_382 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(382)

private:
    class MutatorASTConsumer_382 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_382(Rewriter &R) : TheRewriter(R) {}
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
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Template_Argument_Parameter_Mismatch_382.h"

// ========================================================================================================
#define MUT382_OUTPUT 1

void MutatorFrontendAction_382::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        // Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        // Check if the number of template arguments equals the number of function arguments
        const TemplateArgumentListInfo *TemplateArgs = CE->getTemplateArgs();
        if (!TemplateArgs || TemplateArgs->size() != CE->getNumArgs())
            return;

        // Get the source code of the call
        std::string callStr = stringutils::rangetoStr(*Result.SourceManager, CE->getSourceRange());

        // Find the position of the arguments
        size_t openParen = callStr.find('(');
        if (openParen == std::string::npos)
            return;
        size_t closeParen = callStr.find(')');
        if (closeParen == std::string::npos)
            return;

        std::string argsPart = callStr.substr(openParen + 1, closeParen - openParen - 1);

        // Split arguments by commas
        std::vector<std::string> args;
        boost::split(args, argsPart, boost::is_any_of(","), boost::token_compress_off);
        if (args.empty())
            return;

        // Get the last argument
        std::string lastArg = boost::trim_copy(args.back());
        std::string newArg = ", " + lastArg;

        // Create the new call string
        std::string newCallStr = callStr.substr(0, closeParen) + newArg + callStr.substr(closeParen);

        // Replace the original call with the new one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), newCallStr);
    }
}

void MutatorFrontendAction_382::MutatorASTConsumer_382::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}