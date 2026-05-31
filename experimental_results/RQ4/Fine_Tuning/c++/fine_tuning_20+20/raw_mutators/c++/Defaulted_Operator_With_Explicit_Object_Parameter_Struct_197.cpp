//header file
#pragma once
#include "Mutator_base.h"

/**
 * Defaulted_Operator_With_Explicit_Object_Parameter_Struct_197
 */ 
class MutatorFrontendAction_197 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(197)

private:
    class MutatorASTConsumer_197 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_197(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Defaulted_Operator_With_Explicit_Object_Parameter_Struct_197.h"

// ========================================================================================================
#define MUT197_OUTPUT 1

void MutatorFrontendAction_197::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;

      if (DL->isCompleteDefinition() == false)
        return;
      //Get the source code text of target node
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          return;
        }
      }
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("operator==") != string::npos ||
          content.find("operator!=") != string::npos)
        return;

      auto ins = "bool operator==(this const " + DL->getNameAsString() +
                 " &) const = default;\n";
      ins = "/*mut197*/" + ins;
      //Perform mutation on the source code text by applying string replacement
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), ins);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}

void MutatorFrontendAction_197::MutatorASTConsumer_197::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}