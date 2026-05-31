//source file
#include "../include/namespace_misplacement_in_structs_271.h"

// ========================================================================================================
#define MUT271_OUTPUT 1

void MutatorFrontendAction_271::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
            return;
        
        if (!SD->isStruct() || !SD->isCompleteDefinition())
            return;
        
        if (targetNamespace == nullptr) {
            for (auto &decl : Result.Context->getTranslationUnitDecl()->decls()) {
                if (auto *NS = llvm::dyn_cast<NamespaceDecl>(decl)) {
                    targetNamespace = NS;
                    break;
                }
            }
        }

        if (targetNamespace) {
            std::string namespaceUsage = "\nnamespace using " + targetNamespace->getNameAsString() + "; // Misplaced\n";
            auto structSourceRange = SD->getSourceRange();
            std::string structContent = stringutils::rangetoStr(*(Result.SourceManager), structSourceRange);
            structContent.insert(structContent.find("{") + 1, namespaceUsage);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(structSourceRange), structContent);
        }
    }
}

void MutatorFrontendAction_271::MutatorASTConsumer_271::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}