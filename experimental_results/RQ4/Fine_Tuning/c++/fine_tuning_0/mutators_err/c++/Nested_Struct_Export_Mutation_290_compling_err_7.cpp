//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Struct_Export_Mutation_290
 */ 
class MutatorFrontendAction_290 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(290)

private:
    class MutatorASTConsumer_290 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_290(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Nested_Struct_Export_Mutation_290.h"

// ========================================================================================================
#define MUT290_OUTPUT 1

void MutatorFrontendAction_290::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ExportDecl>("Exported")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto exported = stringutils::rangetoStr(*(Result.SourceManager),
                                              MT->getSourceRange());
      auto decls = MT->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Tag) {
          llvm::outs() << "/*mut290*/" << exported << '\n';
          //Perform mutation on the source code text by applying string replacement
          exported = stringutils::replace_word(exported, "export", "");
          break;
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), exported);
    }
    //Check whether the matched AST node is the target node
    else if (auto *DT = Result.Nodes.getNodeAs<clang::TagDecl>("Nested")) {
      //Filter nodes in header files
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
        return;
      if (DT->isCompleteDefinition() == false)
        return;
      if (DT->isStruct() == false)
        return;
      if (DT->getAccess() != AccessSpecifier::AS_none)
        return;
      //Get the source code text of target node
      auto nested = stringutils::rangetoStr(*(Result.SourceManager),
                                            DT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      nested = "export " + nested;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DT->getSourceRange()), nested);
    }
}
  
void MutatorFrontendAction_290::MutatorASTConsumer_290::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = exportDecl().bind("Exported");
    auto nested_matcher = tagDecl().bind("Nested");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(nested_matcher, &callback);
    matchFinder.matchAST(Context);
}