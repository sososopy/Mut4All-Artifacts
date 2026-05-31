//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Exception_Specifications_259
 */ 
class MutatorFrontendAction_259 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(259)

private:
    class MutatorASTConsumer_259 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_259(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_function_exception_specifications_259.h"

// ========================================================================================================
#define MUT259_OUTPUT 1

void MutatorFrontendAction_259::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithNoexcept")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      // Check if the function has a noexcept specification
      if (FD->hasAttr<clang::NoThrowAttr>()) {
        // Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        // Perform mutation on the source code text by applying string replacement
        std::string helperFunction = "int helperFunction() { return 42; }\n";
        std::string mutatedNoexcept = "noexcept(helperFunction() > 0)";
        size_t noexceptPos = declaration.find("noexcept");
        if (noexceptPos != std::string::npos) {
          declaration.replace(noexceptPos, std::string("noexcept").length(), mutatedNoexcept);
          declaration = helperFunction + declaration;
          // Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_259::MutatorASTConsumer_259::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isNoThrow()).bind("FunctionWithNoexcept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
