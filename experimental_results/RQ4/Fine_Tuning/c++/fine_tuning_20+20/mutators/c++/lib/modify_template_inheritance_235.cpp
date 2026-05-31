//source file
#include "../include/Modify_Template_Inheritance_235.h"

// ========================================================================================================
#define MUT235_OUTPUT 1

void MutatorFrontendAction_235::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto name = DL->getNameAsString();
      auto spec = DL->getTemplatedDecl();
      if (spec->isCompleteDefinition() == false)
        return;
      auto bases = spec->bases();
      if (bases.empty()) {
        base_names.push_back("");
        base_types.push_back(0);
      } else {
        for (auto base : bases) {
          auto basetype = base.getType();
          auto baseptr = basetype.getTypePtr();
          if (baseptr->isDependentType() == false)
            return;
          auto baseclass = basetype.getAsString();
          base_names.push_back(baseclass);
          if (base.isVirtual())
            base_types.push_back(2);
          else
            base_types.push_back(1);
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "MutTemplates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto name = DL->getNameAsString();
      auto spec = DL->getTemplatedDecl();
      if (spec->isCompleteDefinition() == false)
        return;
      auto bases = spec->bases();
      if (bases.empty()) {
        if (base_names.empty())
          return;
        auto index = getrandom::getRandomIndex(base_names.size() - 1);
        if (base_names[index] == "")
          return;
        string newbase = base_names[index];
        if (base_types[index] == 2)
          newbase = "virtual " + newbase;
        newbase = "/*mut235*/: public " + newbase;
        llvm::outs() << newbase << '\n';
        Rewrite.ReplaceText(spec->getEndLoc(), 0, newbase);
      } else {
        auto index = getrandom::getRandomIndex(distance(bases.begin(),bases.end()) - 1);
        auto target = bases.begin();
        for (int i = 0; i < index; i++)
          target++;
        auto targetname = target->getType().getAsString();
        llvm::outs() << targetname << '\n';
        if (base_names.empty())
          return;
        auto newindex = getrandom::getRandomIndex(base_names.size() - 1);
        auto newname = base_names[newindex];
        if (newname == targetname)
          return;
        if (base_types[newindex] == 2)
          newname = "virtual " + newname;
        auto newbase = "/*mut235*/public " + newname;
        Rewrite.ReplaceText(target->getSourceRange(), newbase);
      }
    }
  }

void MutatorFrontendAction_235::MutatorASTConsumer_235::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto mut_matcher = classTemplateDecl().bind("MutTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(mut_matcher, &callback);
    matchFinder.matchAST(Context);
}