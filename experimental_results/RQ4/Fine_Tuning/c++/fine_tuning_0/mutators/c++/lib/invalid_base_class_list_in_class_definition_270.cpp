//source file
#include "../include/Invalid_Base_Class_List_In_Class_Definition_270.h"

// ========================================================================================================
#define MUT270_OUTPUT 1

void MutatorFrontendAction_270::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (DL->getNumBases() == 0)
      return;
    auto bases = DL->bases();
    std::vector<std::string> base_names;
    for (auto base : bases) {
      auto base_type = base.getType();
      if (base_type->isIncompleteType()) {
        llvm::outs() << "incomplete base type\n";
        return;
      }
      auto base_decl = base_type->getAsCXXRecordDecl();
      if (base_decl == nullptr)
        return;
      auto base_name = base_decl->getNameAsString();
      base_names.push_back(base_name);
    }
    size_t index = getrandom::getRandomIndex(base_names.size() - 1);
    llvm::outs() << index << '\n';
    base_names[index] = "mut270";
    std::string new_base_list = ": ";
    for (auto base_name : base_names) {
      new_base_list += base_name + ", ";
    }
    new_base_list.pop_back();
    new_base_list.pop_back();
    llvm::outs() << new_base_list << '\n';
    auto pos = content.find(':');
    if (pos == std::string::npos)
      return;
    auto pos2 = content.find('{');
    if (pos2 == std::string::npos)
      return;
    content.replace(pos, pos2 - pos, new_base_list);
    content = "/*mut270*/class mut270;\n" + content;
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  }
}

void MutatorFrontendAction_270::MutatorASTConsumer_270::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}