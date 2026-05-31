//source file
#include "../include/Insert_Zero_Sized_Bit_Field_451.h"

// ========================================================================================================
#define MUT451_OUTPUT 1

void MutatorFrontendAction_451::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    if (!CL->isCompleteDefinition())
      return;
    cur_classes.push_back(CL);
  } else if (auto *BL = Result.Nodes.getNodeAs<clang::FieldDecl>("BitField")) {
    if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   BL->getLocation()))
      return;
    if (!BL->isBitField())
      return;
    if (BL->getBitWidthValue(*(Result.Context)) == 0) {
      llvm::outs() << "mut451: find zero-sized bitfield\n";
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             BL->getSourceRange());
      llvm::outs() << content << '\n';
      int choice = getrandom::getRandomIndex(1);
      if (choice) {
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BL->getSourceRange()), "");
      } else {
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BL->getSourceRange()), "int mut_451 : 1");
      }
    }
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes2")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (!DL->isCompleteDefinition())
      return;
    if (DL->isEmpty())
      return;
    auto fields = DL->fields();
    for (auto field : fields) {
      if (field->isBitField() &&
          field->getBitWidthValue(*(Result.Context)) == 0) {
        return;
      }
    }
    auto first_field = fields.begin();
    llvm::outs() << "mut451: find class without zero-sized bitfield\n";
    Rewrite.ReplaceText((*first_field)->getBeginLoc(), 0, "int mut_451 : 0; ");
  }
}

void MutatorFrontendAction_451::MutatorASTConsumer_451::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto bitfield_matcher = fieldDecl().bind("BitField");
    auto class_matcher = cxxRecordDecl().bind("Classes2");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(bitfield_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}