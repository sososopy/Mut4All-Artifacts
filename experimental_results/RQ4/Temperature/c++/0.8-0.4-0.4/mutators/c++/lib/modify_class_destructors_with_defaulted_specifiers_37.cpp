//source file
#include "../include/modify_class_destructors_with_defaulted_specifiers_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        //Filter nodes in header files
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
            return;

        if (!CL->isCompleteDefinition())
            return;

        auto *Destructor = CL->getDestructor();
        if (Destructor) {
            if (!Destructor->isDefaulted()) {
                //Get the source code text of target node
                auto dtorRange = Destructor->getSourceRange();
                std::string dtorText = stringutils::rangetoStr(*(Result.SourceManager), dtorRange);
                //Perform mutation on the source code text by applying string replacement
                std::string defaultedDtor = "~" + CL->getNameAsString() + "() = default;";
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(dtorRange), defaultedDtor);
            }
        } else {
            // If destructor does not exist, create a defaulted one
            std::string defaultedDtor = "\n~" + CL->getNameAsString() + "() = default;";
            if (CL->hasDefinition()) {
                for (const auto *D : CL->decls()) {
                    if (const auto *RD = llvm::dyn_cast<clang::CXXRecordDecl>(D)) {
                        if (RD->isCompleteDefinition()) {
                            Rewrite.InsertTextAfterToken(RD->getBraceRange().getEnd(), defaultedDtor);
                            break;
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}