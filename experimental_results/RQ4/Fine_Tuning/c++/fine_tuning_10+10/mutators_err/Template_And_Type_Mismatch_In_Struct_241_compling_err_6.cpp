//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_And_Type_Mismatch_In_Struct_241
 */ 
class MutatorFrontendAction_241 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(241)

private:
    class MutatorASTConsumer_241 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_241(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_241.h"

// ========================================================================================================
#define MUT241_OUTPUT 1

void MutatorFrontendAction_241::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("StaticMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isStatic()) {
        //Get the source code text of target node
        auto returnType = MT->getReturnType().getAsString();
        size_t templateStart = returnType.find('<');
        size_t templateEnd = returnType.find('>');

        if (templateStart != std::string::npos && templateEnd != std::string::npos) {
          std::string originalTemplateArg = returnType.substr(templateStart + 1, templateEnd - templateStart - 1);
          std::string mutatedTemplateArg = "NonExistentType::undefined_subtype";
          std::string mutatedReturnType = returnType.replace(templateStart + 1, templateEnd - templateStart - 1, mutatedTemplateArg);
          
          //Perform mutation on the source code text by applying string replacement
          Rewrite.ReplaceText(MT->getReturnTypeSourceRange(), mutatedReturnType);
        }
      }
    }
}
  
void MutatorFrontendAction_241::MutatorASTConsumer_241::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(hasStaticStorageDuration()).bind("StaticMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}