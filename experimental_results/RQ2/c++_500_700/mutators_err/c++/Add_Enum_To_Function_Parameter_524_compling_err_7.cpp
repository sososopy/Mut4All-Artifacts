//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_enum_to_function_parameter_524
 */ 
class MutatorFrontendAction_524 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(524)

private:
    class MutatorASTConsumer_524 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_524(Rewriter &R) : TheRewriter(R) {}
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
#include "MutatorFrontendAction_524.h"

// ========================================================================================================
#define MUT524_OUTPUT 1

void MutatorFrontendAction_524::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string enumDecl = "enum MyEnum { Value1, Value2 }; ";
      std::string mutatedParam = "enum MyEnum;";
      size_t paramStart = declaration.find("(");
      size_t paramEnd = declaration.find(")");
      if (paramStart != std::string::npos && paramEnd != std::string::npos) {
          declaration.insert(paramEnd, mutatedParam);
          declaration.insert(0, enumDecl);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_524::MutatorASTConsumer_524::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParameter(anything())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}