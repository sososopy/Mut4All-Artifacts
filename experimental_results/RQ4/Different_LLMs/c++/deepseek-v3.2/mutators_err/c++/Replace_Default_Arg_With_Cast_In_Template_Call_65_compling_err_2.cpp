//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Default_Arg_With_Cast_In_Template_Call_65
 */ 
class MutatorFrontendAction_65 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(65)

private:
    class MutatorASTConsumer_65 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_65(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeDecl*> availableTypes;
    };
};

//source file
#include "../include/Replace_Default_Arg_With_Cast_In_Template_Call_65.h"

// ========================================================================================================
#define MUT65_OUTPUT 1

void MutatorFrontendAction_65::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
            return;
        //Record available types for casting
        if (!TD->getTypeForDecl()->isDependentType() && !TD->getTypeForDecl()->isVoidType()) {
            availableTypes.push_back(TD);
        }
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
        //Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
            return;

        // Check if this is a template function call
        const clang::FunctionDecl* FD = CE->getDirectCallee();
        if (!FD || !FD->isTemplateInstantiation())
            return;

        // Check if any argument is a default argument
        bool hasDefaultArg = false;
        int defaultArgIndex = -1;
        for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
            const clang::Expr* arg = CE->getArg(i);
            if (arg->isDefaultArgument()) {
                hasDefaultArg = true;
                defaultArgIndex = i;
                break;
            }
        }

        if (!hasDefaultArg || defaultArgIndex == -1)
            return;

        // Check if we have available types for casting
        if (availableTypes.empty())
            return;

        //Get the source code text of target node
        std::string callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
        const clang::Expr* defaultArgExpr = CE->getArg(defaultArgIndex);
        std::string defaultArgText = stringutils::rangetoStr(*(Result.SourceManager), defaultArgExpr->getSourceRange());

        //Select a random type for casting
        size_t typeIndex = getrandom::getRandomIndex(availableTypes.size() - 1);
        const clang::TypeDecl* selectedType = availableTypes[typeIndex];
        std::string typeName = selectedType->getNameAsString();

        //Perform mutation on the source code text by applying string replacement
        std::string castArgText = "static_cast<" + typeName + ">(" + defaultArgText + ")";
        // Replace the default argument with the cast expression in the call text
        size_t argStartPos = callText.find(defaultArgText);
        if (argStartPos != std::string::npos) {
            callText.replace(argStartPos, defaultArgText.length(), castArgText);
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), "/*mut65*/" + callText);
    }
}
  
void MutatorFrontendAction_65::MutatorASTConsumer_65::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher typeMatcher = clang::ast_matchers::typeDecl().bind("TypeDecl");
    StatementMatcher callMatcher = clang::ast_matchers::callExpr(clang::ast_matchers::hasParent(clang::ast_matchers::anyOf(clang::ast_matchers::compoundStmt(), clang::ast_matchers::ifStmt()))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}