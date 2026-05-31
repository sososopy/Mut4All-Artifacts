//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_With_Empty_Lambda_257
 */ 
class MutatorFrontendAction_257 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(257)

private:
    class MutatorASTConsumer_257 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_257(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> targetFunctions;
    };
};

//source file
#include "../include/replace_function_with_empty_lambda_257.h"

// ========================================================================================================
#define MUT257_OUTPUT 1

void MutatorFrontendAction_257::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody() && !FD->isVirtual() && !FD->isMain()) {
        //Get the source code text of target node
        auto functionName = FD->getNameAsString();
        auto returnType = FD->getReturnType().getAsString();
        auto params = FD->parameters();
        std::string paramList;
        for (size_t i = 0; i < params.size(); ++i) {
          if (i > 0) paramList += ", ";
          paramList += params[i]->getType().getAsString() + " " + params[i]->getNameAsString();
        }

        //Perform mutation on the source code text by applying string replacement
        std::string lambdaReplacement = returnType + " " + functionName + "(" + paramList + ") {\n";
        lambdaReplacement += "/*mut257*/return [](){" + (returnType != "void" ? "return " : "") + "};\n";
        lambdaReplacement += "}\n";

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), lambdaReplacement);
      }
    }
}

void MutatorFrontendAction_257::MutatorASTConsumer_257::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(cxxConstructorDecl())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}