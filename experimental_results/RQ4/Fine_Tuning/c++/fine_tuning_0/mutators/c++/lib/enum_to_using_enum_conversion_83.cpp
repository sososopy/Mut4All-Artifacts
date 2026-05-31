//source file
#include "../include/Enum_to_Using_Enum_Conversion_83.h"

// ========================================================================================================
#define MUT83_OUTPUT 1

void MutatorFrontendAction_83::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      //Get the source code text of target node
      auto name = ED->getNameAsString();
      llvm::outs() << name << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto using_enum_decl = "/*mut83*/using enum " + name + ";\n";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ED->getEndLoc(), 0, using_enum_decl);
      cur_enums.push_back(ED);
    } else if (auto *IL = Result.Nodes.getNodeAs<clang::IntegerLiteral>(
                   "IntegerLiteral")) {
      if (!IL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IL->getBeginLoc()))
        return;

      auto type = IL->getType();
      if (!type->isEnumeralType())
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          IL->getSourceRange());
      llvm::outs() << text << '\n';
      Rewrite.ReplaceText(IL->getSourceRange(), "/*mut83*/" + text);
    } else if (auto *ME = Result.Nodes.getNodeAs<clang::MemberExpr>(
                   "MemberExpr")) {
      if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ME->getBeginLoc()))
        return;

      auto base = ME->getBase();
      auto base_type = base->getType();
      if (!base_type->isEnumeralType())
        return;
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          ME->getSourceRange());
      llvm::outs() << text << '\n';
      auto member_name = ME->getMemberNameInfo().getAsString();
      Rewrite.ReplaceText(ME->getSourceRange(), "/*mut83*/" + member_name);
    }
}
  
void MutatorFrontendAction_83::MutatorASTConsumer_83::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto enum_matcher = enumDecl().bind("Enums");
    auto integer_matcher = integerLiteral().bind("IntegerLiteral");
    auto member_matcher = memberExpr().bind("MemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.addMatcher(integer_matcher, &callback);
    matchFinder.addMatcher(member_matcher, &callback);
    matchFinder.matchAST(Context);
}