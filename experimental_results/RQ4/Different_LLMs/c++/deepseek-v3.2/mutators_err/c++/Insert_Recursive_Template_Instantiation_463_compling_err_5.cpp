//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_463
 */ 
class MutatorFrontendAction_463 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(463)

private:
    class MutatorASTConsumer_463 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_463(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_463.h"

// ========================================================================================================
#define MUT463_OUTPUT 1

void MutatorFrontendAction_463::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MemberFunc")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if method is a template and within a class template
      if (!MT->isTemplated() || !MT->getParent()->isTemplateDecl())
        return;
      //Get the source code text of target node
      if (!MT->hasBody())
        return;
      auto bodyRange = MT->getBody()->getSourceRange();
      auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
      //Find the opening brace position
      size_t bracePos = bodyText.find('{');
      if (bracePos == std::string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string mutation = "\n        if constexpr (false) {\n            auto recursive_lambda = [this](const auto& dummy) -> void {\n                using NewType = decltype(dummy + sizeof(";
      //Get class template name
      auto classDecl = MT->getParent();
      std::string className = classDecl->getNameAsString();
      mutation += className + "));\n                this->" + MT->getNameAsString() + "<NewType>(dummy);\n            };\n            recursive_lambda(0);\n        }\n";
      //Insert mutation after opening brace
      bodyText.insert(bracePos + 1, mutation);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
    }
}
  
void MutatorFrontendAction_463::MutatorASTConsumer_463::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isTemplate(), hasParent(cxxRecordDecl(isClassTemplate()))).bind("MemberFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}