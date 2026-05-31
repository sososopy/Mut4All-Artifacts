//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Noexcept_Specifier_With_Complex_Expression_11
 */ 
class MutatorFrontendAction_11 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(11)

private:
    class MutatorASTConsumer_11 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_11(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> targetFunctions;
    };
};

//source file
#include "../include/Mutator_Replace_Noexcept_Specifier_With_Complex_Expression_11.h"

// ========================================================================================================
#define MUT11_OUTPUT 1

void MutatorFrontendAction_11::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the2 matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TargetFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if function has noexcept specifier
      if (FD->getExceptionSpecType() != EST_Noexcept)
        return;
      //Check if function is a template
      if (!FD->getDescribedFunctionTemplate())
        return;
      //Record candidate function
      targetFunctions.push_back(FD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("SelectedFunction")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->getExceptionSpecType() != EST_Noexcept)
        return;
      if (!FD->getDescribedFunctionTemplate())
        return;
      //Get the source code text of target node
      SourceRange funcRange = FD->getSourceRange();
      std::string funcText = stringutils::rangetoStr(*(Result.SourceManager), funcRange);
      
      //Extract template parameters
      std::string templateParams = "";
      if (FD->getDescribedFunctionTemplate()) {
        auto *Template = FD->getDescribedFunctionTemplate();
        SourceRange templateRange = Template->getSourceRange();
        std::string templateText = stringutils::rangetoStr(*(Result.SourceManager), templateRange);
        size_t templateEnd = templateText.find('>');
        if (templateEnd != std::string::npos) {
          templateParams = templateText.substr(0, templateEnd+1);
        }
      }
      
      //Get function name
      std::string funcName = FD->getNameAsString();
      
      //Get parameter types
      std::string paramTypes = "";
      unsigned paramCount = FD->getNum2Params();
      for (unsigned i = 0; i < paramCount; ++i) {
        const ParmVarDecl *Param = FD->getParamDecl(i);
        paramTypes += Param->getType().getAsString();
        if (i < paramCount - 1) paramTypes += ", ";
      }
      
      //Find noexcept specifier location
      SourceLocation noexceptLoc = FD->getExceptionSpecSourceRange().getBegin();
      if (noexceptLoc.isInvalid()) return;
      
      //Build new noexcept expression
      std::string newNoexceptExpr;
      if (FD->isCXXClassMember()) {
        //Member function: use std::declval<Class<T...>>()
        const CXXRecordDecl *ClassDecl = dyn_cast<CXXRecordDecl>(FD->getDeclContext());
        std::string className = ClassDecl->getNameAsString();
        newNoexceptExpr = "noexcept(std::declval<" + className + templateParams.substr(templateParams.find('<')) + ">()." + funcName + "(std::declval<" + paramTypes + ">()...))";
      } else {
        //Free function: create wrapper
        newNoexceptExpr = "noexcept(std::declval<Wrapper" + templateParams.substr(templateParams.find('<')) + ">().call())";
      }
      
      //Perform mutation on the source code text by applying string replacement
      //Replace noexcept specifier
      SourceRange noexceptRange = FD->getExceptionSpecSourceRange();
      std::string noexceptText = stringutils::rangetoStr(*(Result.SourceManager), noexceptRange);
      size_t noexceptPos = funcText.find(noexceptText);
      if (noexceptPos != std::string::npos) {
        funcText.replace(noexceptPos, noexceptText.length(), newNoexceptExpr);
      }
      
      //Add static_assert after function
      std::string staticAssert;
      if (FD->isCXXClassMember()) {
        const CXXRecordDecl *ClassDecl = dyn_cast<CXXRecordDecl>(FD->getDeclContext());
        std::string className = ClassDecl->getNameAsString();
        staticAssert = "\nstatic_assert(noexcept(std::declval<" + className + templateParams.substr(templateParams.find('<')) + ">()." + funcName + "(std::declval<" + paramTypes + ">()...)), \"noexcept check\");";
      } else {
        staticAssert = "\nstatic_assert(noexcept(Wrapper" + templateParams.substr(templateParams.find('<')) + "::call()), \"noexcept check\");";
      }
      
      //Insert wrapper struct for free functions
      if (!FD->isCXXClassMember()) {
        std::string wrapperStruct = "\nstruct Wrapper" + templateParams.substr(templateParams.find('<')) + " {\n  static auto call() -> decltype(" + funcName + templateParams.substr(templateParams.find('<')) + "(std::declval<" + paramTypes + ">()...)) {\n    return " + funcName + templateParams.substr(templateParams.find('<')) + "(std::declval<" + paramTypes + ">()...);\n  }\n};";
        funcText += wrapperStruct;
      }
      
      funcText += staticAssert;
      funcText = "/*mut11*/" + funcText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(funcRange), funcText);
    }
}
  
void MutatorFrontendAction_11::MutatorASTConsumer_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplate(), hasTrailingReturn()).bind("TargetFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
    
    //Select one function from recorded candidates for mutation
    if (!callback.targetFunctions.empty()) {
      size_t selectedIdx = getrandom::getRandomIndex(callback.targetFunctions.size() - 1);
      const FunctionDecl *selectedFunc = callback.targetFunctions[selectedIdx];
      
      MatchFinder secondFinder;
      DeclarationMatcher secondMatcher = functionDecl(isTemplate(), hasTrailingReturn(), hasName(selectedFunc->getNameAsString())).bind("SelectedFunction");
      secondFinder.addMatcher(secondMatcher, &callback);
      secondFinder.matchAST(Context);
    }
}