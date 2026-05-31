//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Uninstantiated_Exception_Spec_In_Templated_Function_57
 */ 
class MutatorFrontendAction_57 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(57)

private:
    class MutatorASTConsumer_57 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_57(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Introduce_Uninstantiated_Exception_Spec_In_Templated_Function_57.h"

// ========================================================================================================
#define MUT57_OUTPUT 1

void MutatorFrontendAction_57::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplatedFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isTemplated())
        return;
      auto *FT = FD->getType()->getAs<clang::FunctionProtoType>();
      if (!FT)
        return;
      if (FT->hasExceptionSpec())
        return;
      if (FD->getNumParams() == 0)
        return;
      //Get the source code text of target node
      auto funcText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = funcText.find('(');
      if (pos == std::string::npos)
        return;
      std::string paramName = FD->getParamDecl(0)->getNameAsString();
      if (paramName.empty())
        paramName = "param0";
      std::string noexceptSpec = " noexcept(noexcept(std::move(" + paramName + ")))";
      funcText.insert(pos, noexceptSpec);
      funcText = "/*mut57*/" + funcText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
    }
}
  
void MutatorFrontendAction_57::MutatorASTConsumer_57::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), unless(cxxConstructorDecl()), unless(cxxDestructorDecl())).bind("TemplatedFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}