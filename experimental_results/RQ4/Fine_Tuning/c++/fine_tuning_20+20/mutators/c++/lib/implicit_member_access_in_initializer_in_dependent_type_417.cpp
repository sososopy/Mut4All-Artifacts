//source file
#include "../include/implicit_member_access_in_initializer_in_dependent_type_417.h"

// ========================================================================================================
#define MUT417_OUTPUT 1

void MutatorFrontendAction_417::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      cur_templates.push_back(TP);
    } else if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Structs")) { // Find all structs in the template
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (ST->isStruct() == false)
        return;
      if (ST->isCompleteDefinition() == false)
        return;
      for (auto tp : cur_templates) {
        if (ST->getEnclosingNamespaceContext() == tp->getDeclContext()) {
          llvm::outs() << "Mut417: Find target struct in template!\n";
          auto fields = ST->fields();
          for (auto field : fields) {
            auto fieldname = field->getNameAsString();
            auto type = field->getType();
            auto type_str = type.getAsString();
            std::string replacement =
                type_str + " " + fieldname + " = " + fieldname;
            llvm::outs() << "Mut417: replacement: " << replacement << '\n';
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                    field->getSourceRange()),
                                replacement);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_417::MutatorASTConsumer_417::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto template_matcher = classTemplateDecl().bind("Templates");
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.matchAST(Context);
}