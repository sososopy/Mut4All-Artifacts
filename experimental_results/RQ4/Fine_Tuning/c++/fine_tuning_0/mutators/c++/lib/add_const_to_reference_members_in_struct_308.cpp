//source file
#include "../include/Add_Const_To_Reference_Members_In_Struct_308.h"

// ========================================================================================================
#define MUT308_OUTPUT 1

void MutatorFrontendAction_308::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    llvm::outs() << content;
    auto fields = DL->fields();
    int has_reference = 0;
    for (auto field : fields) {
      if (field->getType()->isReferenceType()) {
        has_reference = 1;
        auto field_content = stringutils::rangetoStr(
            *(Result.SourceManager), field->getSourceRange());
        if (field_content.find("const") == string::npos) {
          content.insert(field->getBeginLoc().getRawEncoding() -
                             DL->getBeginLoc().getRawEncoding(),
                         "const ");
        }
      }
    }
    if (has_reference == 0) {
      auto types = DL->decls();
      for (auto type : types) {
        if (type->getKind() == Decl::Kind::Typedef) {
          auto typedef_type = dyn_cast<TypedefDecl>(type);
          auto type_content = stringutils::rangetoStr(
              *(Result.SourceManager), typedef_type->getSourceRange());
          llvm::outs() << type_content;
          if (typedef_type->getUnderlyingType()->isReferenceType()) {
            content.insert(content.rfind('}'), type_content + " mut_308;\n");
          }
        }
      }
    }
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    cur_structs.push_back(DL);
    has_ref.push_back(has_reference);
  }
}

void MutatorFrontendAction_308::MutatorASTConsumer_308::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl(isStruct()).bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}