//source file
#include "../include/modify_union_usage_in_structs_576.h"

// ========================================================================================================
#define MUT576_OUTPUT 1

void MutatorFrontendAction_576::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("structWithUnion")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;

        for (auto *D : RD->decls()) {
            if (auto *UD = llvm::dyn_cast<clang::RecordDecl>(D)) {
                if (UD->isUnion() && UD->isCompleteDefinition()) {
                    std::string newType = "double";
                    std::string newMemberName = "c";
                    std::string newMember = newType + " " + newMemberName + ";";
                    SourceLocation insertLoc = UD->getEndLoc();
                    Rewrite.InsertTextBefore(insertLoc, newMember);

                    for (auto *method : RD->methods()) {
                        if (method->hasBody()) {
                            auto bodyRange = method->getBody()->getSourceRange();
                            std::string methodBody = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
                            size_t pos = methodBody.find("{");
                            if (pos != std::string::npos) {
                                std::string initStatement = "\n" + newMemberName + " = 20.0;";
                                methodBody.insert(pos + 1, initStatement);
                                Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), methodBody);
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_576::MutatorASTConsumer_576::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxRecordDecl(isUnion()))).bind("structWithUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}