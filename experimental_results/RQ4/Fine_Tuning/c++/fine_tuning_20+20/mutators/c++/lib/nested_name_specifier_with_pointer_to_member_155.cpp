//source file
#include "../include/nested_name_specifier_with_pointer_to_member_155.h"

// ========================================================================================================
#define MUT155_OUTPUT 1

void MutatorFrontendAction_155::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      auto fields = CL->fields();
      for (auto field : fields) {
        if (field->getType()->isMemberPointerType()) {
          cur_field = field;
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclStmt>("Decls")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (cur_field == nullptr)
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (auto vardecl = dyn_cast<clang::VarDecl>(decl)) {
          if (vardecl->getType() == cur_field->getType()) {
            auto name = vardecl->getNameAsString();
            auto fieldname = cur_field->getNameAsString();
            auto classname = cur_field->getParent()->getNameAsString();
            auto ins = "/*mut155*/" + name + "->" + classname + " :: " +
                       fieldname + ";\n";
            Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_155::MutatorASTConsumer_155::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto decl_matcher = declStmt().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}