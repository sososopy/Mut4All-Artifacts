//source file
#include "../include/Alter_Class_Template_Specialization_240.h"

// ========================================================================================================
#define MUT240_OUTPUT 1

void MutatorFrontendAction_240::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("Specializations")) {
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      cur_specializations.push_back(TS);
    } else if (auto *UA = Result.Nodes.getNodeAs<clang::UsingDecl>("Usings")) {
      if (!UA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UA->getLocation()))
        return;
      if (cur_specializations.empty())
        return;
      auto index = getrandom::getRandomIndex(cur_specializations.size() - 1);
      auto target = cur_specializations[index];
      auto target_type = target->getNameAsString();
      auto using_name = UA->getNameAsString();
      if (target_type != using_name)
        return;
      llvm::outs() << target_type << '\n';
      auto args = target->getTemplateArgs().asArray();
      if (args.size() == 0)
        return;
      auto arg = args[0];
      if (arg.getKind() == TemplateArgument::Type) {
        auto type = arg.getAsType();
        if (type->isIntegralType(*Result.Context)) {
          llvm::outs() << "Mut240: Integral Type\n";
          auto type_str = type.getAsString();
          llvm::outs() << type_str << '\n';
          if (type_str == "int") {
            Rewrite.ReplaceText(UA->getBeginLoc(), 0, "/*mut240*/using " +
                                                      using_name +
                                                      " = A<double>;\n");
          } else if (type_str == "long") {
            Rewrite.ReplaceText(UA->getBeginLoc(), 0, "/*mut240*/using " +
                                                      using_name +
                                                      " = A<short>;\n");
          }
        }
      }
    }
}
  
void MutatorFrontendAction_240::MutatorASTConsumer_240::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto specialization_matcher =
        classTemplateSpecializationDecl().bind("Specializations");
    auto using_matcher = usingDecl().bind("Usings");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(specialization_matcher, &callback);
    matchFinder.addMatcher(using_matcher, &callback);
    matchFinder.matchAST(Context);
}