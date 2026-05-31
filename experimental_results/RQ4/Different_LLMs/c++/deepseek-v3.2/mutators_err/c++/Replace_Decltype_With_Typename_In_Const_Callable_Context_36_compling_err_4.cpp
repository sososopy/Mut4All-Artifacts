//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_With_Typename_In_Const_Callable_Context_36
 */ 
class MutatorFrontendAction_36 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(36)

private:
    class MutatorASTConsumer_36 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_36(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DecltypeType>("DecltypeType")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto range = MT->getSourceRange();
      std::string originalText = stringutils::rangetoStr(SM, range);
      
      //Check if it contains std::declval<const T>
      if (originalText.find("std::declval<const") == std::string::npos &&
          originalText.find("declval<const") == std::string::npos)
        return;
      
      //Get the underlying expression
      auto *underlyingExpr = MT->getUnderlyingExpr();
      if (!underlyingExpr) return;
      
      //Check if we're in a template argument deduction context
      auto *parent = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecialization");
      if (!parent) return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "typename std::decay_t<" + originalText + ">::type";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(range, mutatedText);
    }
}
  
void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = templateSpecializationType(
        hasAnyTemplateArgument(hasType(decltypeType(
            hasDescendant(callExpr(hasArgument(
                0, hasType(hasCanonicalType(referenceType(pointee(
                    qualType(isConstQualified(), 
                    hasDeclaration(namedDecl()))))))))))))
            .bind("DecltypeType"))
        .bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}