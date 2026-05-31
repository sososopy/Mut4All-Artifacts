//source file
#include "../include/Template_Aliases_with_Invalid_CTAD_369.h"

// ========================================================================================================
#define MUT369_OUTPUT 1

void MutatorFrontendAction_369::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Aliases")) {
      //Filter nodes in header files
      if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AL->getLocation()))
        return;
      //Get the source code text of target node
      auto T = AL->getTypeSourceInfo()->getType();
      if (T->getTypeClass() != Type::TypeClass::TemplateSpecialization)
        return;
      auto TT = T->getAs<TemplateSpecializationType>();
      if (TT->getNumArgs() == 0)
        return;
      //Record the node information to be used in the mutation process
      cur_alias.push_back(AL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclaratorDecl>( "Decls")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getType()->getTypeClass() != Type::TypeClass::Elaborated)
        return;
      auto T = DL->getType()->getAs<ElaboratedType>();
      if (T->getNamedType()->getTypeClass() != Type::TypeClass::TemplateSpecialization)
        return;
      auto TT = T->getNamedType()->getAs<TemplateSpecializationType>();
      if (TT->getNumArgs() != 0)
        return;
      auto name = DL->getNameAsString();
      for (auto alias : cur_alias) {
        if (alias->getNameAsString() == name) {
          //Get the source code text of target node
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 DL->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          content += "(1)";
          content = "/*mut369*/" + content;
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
        }
      }
    }
}
  
void MutatorFrontendAction_369::MutatorASTConsumer_369::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto alias_matcher = typeAliasDecl().bind("Aliases");
    auto decl_matcher = declaratorDecl().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(alias_matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}