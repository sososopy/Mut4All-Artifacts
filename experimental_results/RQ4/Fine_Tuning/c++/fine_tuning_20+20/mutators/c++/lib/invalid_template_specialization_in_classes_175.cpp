//source file
#include "../include/mutator_175.h"

// ========================================================================================================
#define MUT175_OUTPUT 1

void MutatorFrontendAction_175::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      cur_classes.push_back(DL);
    } else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody() && !MT->isVirtual()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        llvm::outs() << declaration << '\n';
        size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
        auto target = cur_classes[index];
        auto target_name = target->getNameAsString();
        if (target_name == "Mut_175")
          return;
        if (target->isCompleteDefinition() == false)
          return;
        if (target->isLambda())
          return;
        if (target->isStruct() == false && target->isClass() == false)
          return;
        if (target->isStruct() == false && target->isClass() == false)
          return;
        auto fields = target->fields();
        if (fields.begin() == fields.end())
          return;
        auto field = fields.begin();
        std::advance(field, getrandom::getRandomIndex(std::distance(fields.begin(), fields.end()) - 1));
        auto field_name = field->getNameAsString();
        auto ins = target_name + " :: " + field_name;
        llvm::outs() << ins << '\n';
        declaration.insert(declaration.find("{") + 1, "\n/*mut175*/" + ins + ";");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
  }
  
void MutatorFrontendAction_175::MutatorASTConsumer_175::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto method_matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(method_matcher, &callback);
    matchFinder.matchAST(Context);
}