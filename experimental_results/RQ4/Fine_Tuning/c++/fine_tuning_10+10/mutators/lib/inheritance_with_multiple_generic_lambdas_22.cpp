//source file
#include "../include/inheritance_with_multiple_generic_lambdas_22.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;

        if (SD->hasDefinition() && SD->isStruct()) {
            auto constructor = std::find_if(SD->ctor_begin(), SD->ctor_end(),
                [](const CXXConstructorDecl *ctor) { return ctor->isUserProvided(); });

            if (constructor != SD->ctor_end()) {
                std::string structText = stringutils::rangetoStr(*(Result.SourceManager), SD->getSourceRange());
                std::string newLambda = "auto newLambda = [](auto ...args) { /* empty lambda function */ };";
                std::string methodText = "newLambda(args...);";

                size_t insertPos = structText.find("{");
                if (insertPos != std::string::npos) {
                    structText.insert(insertPos + 1, "\n" + newLambda + "\n");
                }

                for (auto method : SD->methods()) {
                    if (method->hasBody()) {
                        std::string methodBody = stringutils::rangetoStr(*(Result.SourceManager), method->getBody()->getSourceRange());
                        size_t methodInsertPos = methodBody.rfind("}");
                        if (methodInsertPos != std::string::npos) {
                            methodBody.insert(methodInsertPos, "\n" + methodText + "\n");
                            Rewrite.ReplaceText(CharSourceRange::getTokenRange(method->getSourceRange()), methodBody);
                        }
                    }
                }

                Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()), structText);
            }
        }
    }
}
  
void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}